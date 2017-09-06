#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include "middleware.h"

/* Route */
typedef struct _Route {
	HTTPMethod method;
	char *uri;
	SAF saf;
} Route;

Route routes[MAX_HTTP_ROUTES];
int routes_used = 0;

const char* GetContentType(const char *path)
{
	if(strstr(path,".HTML") || strstr(path,".html")) return "text/html";
	if(strstr(path,".JPEG") || strstr(path,".jpeg")) return "image/jpeg";
	if(strstr(path,".PNG" ) || strstr(path,".png" )) return "image/png";
	if(strstr(path,".TXT" ) || strstr(path,".txt" )) return "text";
	if(strstr(path,".JPG" ) || strstr(path,".jpg" )) return "image/jpg";
	if(strstr(path,".CSS" ) || strstr(path,".css" )) return "text/css";
	if(strstr(path,".DB"  ) || strstr(path,".db"  )) return "text/plain";
	if(strstr(path,".JS"  ) || strstr(path,".js"  )) return "application/javascript";
	if(strstr(path,".XML" ) || strstr(path,".xml" )) return "application/xml";
	return "text/html";
}

const char* GetFileExtension(const char *path)
{
	if(strstr(path,".HTML") || strstr(path,".html")) return "html";
	if(strstr(path,".JPEG") || strstr(path,".jpeg")) return "jpeg";
	if(strstr(path,".PNG" ) || strstr(path,".png" )) return "png";
	if(strstr(path,".TXT" ) || strstr(path,".txt" )) return "txt";
	if(strstr(path,".JPG" ) || strstr(path,".jpg" )) return "jpg";
	if(strstr(path,".CSS" ) || strstr(path,".css" )) return "css";
	if(strstr(path,".JS"  ) || strstr(path,".js"  )) return "js";
	if(strstr(path,".XML" ) || strstr(path,".xml" )) return "xml";
	return "text/html";
}

/* Add an URI and the corresponding server application function into the route
   table. */
int AddRouteHandler(HTTPMethod method, char *uri, SAF saf) {
	if(routes_used < MAX_HTTP_ROUTES) {
		routes[routes_used].method = method;
		routes[routes_used].uri = uri;
		routes[routes_used].saf = saf;
		routes_used++;

		return routes_used;
	}
	else {
		return 0;
	}
}

/* Try to read static files under static folder. */
uint8_t _ReadStaticFiles(HTTPReqMessage *req, HTTPResMessage *res) {
	uint8_t found = 0;
	int8_t depth = 0;
	char *uri = req->Header.URI;
	size_t n = strlen(uri);
	size_t i;

	FILE *fp;
	long int size;
	char path[256] = {STATIC_FILE_FOLDER};
	char * contentType = GetContentType(uri);
	char * fileExtension = GetFileExtension(uri);

//	if (strcmp(fileExtension, "js")==0)
//	{
//		strstr(path, "/scripts");
//	}
//	else if (strcmp(fileExtension, "css")==0)
//	{
//		strstr(path, "/css");
//	}

	strstr(path, uri);
//	char header[] = "HTTP/1.1 200 OK\r\nConnection: close\r\n"
//	                "Content-Type: text/html; charset=UTF-8\r\n\r\n";

	char header[128] = {0};//"HTTP/1.1 200 OK\r\nConnection: close\r\n""Content-Type: ";//text/html; charset=UTF-8\r\n\r\n";
	strcpy(header, "HTTP/1.1 200 OK\r\nConnection: close\r\n Content-Type: ");
//	char *tempHead, *header;
//	*tempHead = '\0';
//	*header = '\0';
//	char *tempHead = strcat("HTTP/1.1 200 OK\r\nConnection: close\r\n Content-Type: ", fileExt);
	strcat(header, contentType);
	strcat(header, "; charset=UTF-8\r\n\r\n");

	/* Prevent Path Traversal. */
	for(i=0; i<n; i++) {
		if(uri[i] == '/') {
			if(((n-i) > 2) && (uri[i+1] == '.') && ((uri[i+2] == '.'))) {
				depth -= 1;
				if(depth < 0)
					break;
			}
			else if (((n-i) > 1) && (uri[i+1] == '.'))
				continue;
			else
				depth += 1;
		}
	}

	if((depth >= 0) && (uri[i-1] != '/')) {
		/* Try to open and load the static file. */
		memcpy(path + strlen(path), uri, strlen(uri));
		fp = fopen(path, "r");
		if(fp != NULL) {
			fseek(fp, 0, SEEK_END);
			size = ftell(fp);
			fseek(fp, 0, SEEK_SET);

//			if(size < MAX_BODY_SIZE) {
				/* Build HTTP OK header. */
				n = strlen(header);
				memcpy(res->_buf, header, n);
				i = n;

				/* Build HTTP body. */
				n = fread(res->_buf + i, 1, size, fp);
				i += n;

				res->_index = i;

				found = 1;
//			}
			fclose(fp);
		}
	}

	return found;
}

void _NotFound(HTTPReqMessage *req, HTTPResMessage *res) {
	uint8_t n;
	char header[] = "HTTP/1.1 404 Not Found\r\nConnection: close\r\n\r\n";

	/* Build HTTP OK header. */
	n = strlen(header);
	memcpy(res->_buf, header, n);
	res->_index = n;
}

/* Dispatch an URI according to the route table. */
void Dispatch(HTTPReqMessage *req, HTTPResMessage *res) {
	uint16_t i;
	size_t n;
	char *req_uri = req->Header.URI;
	uint8_t found = 0;

	/* Check the routes. */
	for(i=0; i<routes_used; i++) {
		/* Compare method. */
		if(req->Header.Method == routes[i].method) {
			/* Compare URI. */
			n = strlen(routes[i].uri);
			if(memcmp(req_uri, routes[i].uri, n) == 0)
				found = 1;
			else
				continue;

			if((found == 1) && ((req_uri[n] == '\0') || (req_uri[n] == '\?'))) {
				/* Found and dispatch the server application function. */
				routes[i].saf(req, res);
				break;
			}
			else {
				found = 0;
			}
		}
	}

	/* Check static files. */
	if(found != 1)
		found = _ReadStaticFiles(req, res);

	/* It is really not found. */
	if(found != 1)
		_NotFound(req, res);
}
