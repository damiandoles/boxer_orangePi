/*
 * server_middleware.c
 *
 *  Created on: Sep 28, 2017
 *      Author: dolewdam
 */


#include <stdlib.h>
#include "server_middleware.h"
#include "http_request.h"
#include "uart.h"

void TerminateApp(int status);
int log_message(const struct mg_connection *conn, const char *message);

#ifdef USE_SSL_DH
#include "openssl/ssl.h"
#include "openssl/dh.h"
#include "openssl/ec.h"
#include "openssl/evp.h"
#include "openssl/ecdsa.h"

DH *
get_dh2236()
{
	static unsigned char dh2236_p[] = {
	    0x0E, 0x97, 0x6E, 0x6A, 0x88, 0x84, 0xD2, 0xD7, 0x55, 0x6A, 0x17, 0xB7,
	    0x81, 0x9A, 0x98, 0xBC, 0x7E, 0xD1, 0x6A, 0x44, 0xB1, 0x18, 0xE6, 0x25,
	    0x3A, 0x62, 0x35, 0xF0, 0x41, 0x91, 0xE2, 0x16, 0x43, 0x9D, 0x8F, 0x7D,
	    0x5D, 0xDA, 0x85, 0x47, 0x25, 0xC4, 0xBA, 0x68, 0x0A, 0x87, 0xDC, 0x2C,
	    0x33, 0xF9, 0x75, 0x65, 0x17, 0xCB, 0x8B, 0x80, 0xFE, 0xE0, 0xA8, 0xAF,
	    0xC7, 0x9E, 0x82, 0xBE, 0x6F, 0x1F, 0x00, 0x04, 0xBD, 0x69, 0x50, 0x8D,
	    0x9C, 0x3C, 0x41, 0x69, 0x21, 0x4E, 0x86, 0xC8, 0x2B, 0xCC, 0x07, 0x4D,
	    0xCF, 0xE4, 0xA2, 0x90, 0x8F, 0x66, 0xA9, 0xEF, 0xF7, 0xFC, 0x6F, 0x5F,
	    0x06, 0x22, 0x00, 0xCB, 0xCB, 0xC3, 0x98, 0x3F, 0x06, 0xB9, 0xEC, 0x48,
	    0x3B, 0x70, 0x6E, 0x94, 0xE9, 0x16, 0xE1, 0xB7, 0x63, 0x2E, 0xAB, 0xB2,
	    0xF3, 0x84, 0xB5, 0x3D, 0xD7, 0x74, 0xF1, 0x6A, 0xD1, 0xEF, 0xE8, 0x04,
	    0x18, 0x76, 0xD2, 0xD6, 0xB0, 0xB7, 0x71, 0xB6, 0x12, 0x8F, 0xD1, 0x33,
	    0xAB, 0x49, 0xAB, 0x09, 0x97, 0x35, 0x9D, 0x4B, 0xBB, 0x54, 0x22, 0x6E,
	    0x1A, 0x33, 0x18, 0x02, 0x8A, 0xF4, 0x7C, 0x0A, 0xCE, 0x89, 0x75, 0x2D,
	    0x10, 0x68, 0x25, 0xA9, 0x6E, 0xCD, 0x97, 0x49, 0xED, 0xAE, 0xE6, 0xA7,
	    0xB0, 0x07, 0x26, 0x25, 0x60, 0x15, 0x2B, 0x65, 0x88, 0x17, 0xF2, 0x5D,
	    0x2C, 0xF6, 0x2A, 0x7A, 0x8C, 0xAD, 0xB6, 0x0A, 0xA2, 0x57, 0xB0, 0xC1,
	    0x0E, 0x5C, 0xA8, 0xA1, 0x96, 0x58, 0x9A, 0x2B, 0xD4, 0xC0, 0x8A, 0xCF,
	    0x91, 0x25, 0x94, 0xB4, 0x14, 0xA7, 0xE4, 0xE2, 0x1B, 0x64, 0x5F, 0xD2,
	    0xCA, 0x70, 0x46, 0xD0, 0x2C, 0x95, 0x6B, 0x9A, 0xFB, 0x83, 0xF9, 0x76,
	    0xE6, 0xD4, 0xA4, 0xA1, 0x2B, 0x2F, 0xF5, 0x1D, 0xE4, 0x06, 0xAF, 0x7D,
	    0x22, 0xF3, 0x04, 0x30, 0x2E, 0x4C, 0x64, 0x12, 0x5B, 0xB0, 0x55, 0x3E,
	    0xC0, 0x5E, 0x56, 0xCB, 0x99, 0xBC, 0xA8, 0xD9, 0x23, 0xF5, 0x57, 0x40,
	    0xF0, 0x52, 0x85, 0x9B,
	};
	static unsigned char dh2236_g[] = {
	    0x02,
	};
	DH *dh;

	if ((dh = DH_new()) == NULL)
		return (NULL);
	dh->p = BN_bin2bn(dh2236_p, sizeof(dh2236_p), NULL);
	dh->g = BN_bin2bn(dh2236_g, sizeof(dh2236_g), NULL);
	if ((dh->p == NULL) || (dh->g == NULL)) {
		DH_free(dh);
		return (NULL);
	}
	return (dh);
}
#endif


#ifndef NO_SSL
int
init_ssl(void *ssl_context, void *user_data)
{
	/* Add application specific SSL initialization */
	struct ssl_ctx_st *ctx = (struct ssl_ctx_st *)ssl_context;

#ifdef USE_SSL_DH
	/* example from https://github.com/civetweb/civetweb/issues/347 */
	DH *dh = get_dh2236();
	if (!dh)
		return -1;
	if (1 != SSL_CTX_set_tmp_dh(ctx, dh))
		return -1;
	DH_free(dh);

	EC_KEY *ecdh = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
	if (!ecdh)
		return -1;
	if (1 != SSL_CTX_set_tmp_ecdh(ctx, ecdh))
		return -1;
	EC_KEY_free(ecdh);

	printf("ECDH ciphers initialized\n");
#endif
	return 0;
}
#endif

void ServerHttp_Init(void)
{
	const char *options[] = {
	    "document_root",
	    DOCUMENT_ROOT,
	    "listening_ports",
	    SERVER_PORT,
	    "request_timeout_ms",
	    "10000",
	    "error_log_file",
	    "error.log",
#ifndef NO_SSL
	    "ssl_certificate",
	    "settings/server.pem",
	    "ssl_protocol_version",
	    "3",
	    "ssl_cipher_list",
#ifdef USE_SSL_DH
	    "ECDHE-RSA-AES256-GCM-SHA384:DES-CBC3-SHA:AES128-SHA:AES128-GCM-SHA256",
#else
	    "DES-CBC3-SHA:AES128-SHA:AES128-GCM-SHA256",
#endif
#endif
	    "enable_auth_domain_check",
	    "no",
	    0};

	struct mg_callbacks callbacks = {0};
	struct mg_context *ctx = {0};
	struct mg_server_ports ports[32] = {0};
	int port_cnt = 0, n = 0;

#ifndef NO_SSL
	if (!mg_check_feature(2))
	{
		fprintf(stderr,
		        "Error: Embedded example built with SSL support, "
		        "but civetweb library build without.\n");
		int err = 1;

		if (err)
		{
			fprintf(stderr, "Cannot start CivetWeb - inconsistent build.\n");
			return EXIT_FAILURE;
		}
	}
#endif

	/* Start CivetWeb web server */
	memset(&callbacks, 0, sizeof(callbacks));
#ifndef NO_SSL
	callbacks.init_ssl = init_ssl;
#endif
	callbacks.log_message = log_message;
	ctx = mg_start(&callbacks, 0, options);

	/* Check return value: */
	if (ctx == NULL)
	{
		fprintf(stderr, "Cannot start CivetWeb - mg_start failed.\n");
		TerminateApp(EXIT_FAILURE);
	}

	HttpReq_RegisterUserHandlers(ctx);

	/* List all listening ports */
	memset(ports, 0, sizeof(ports));
	port_cnt = mg_get_server_ports(ctx, 32, ports);
	printf("\n%i listening ports:\n\n", port_cnt);

	for (n = 0; n < port_cnt && n < 32; n++)
	{
		const char *proto = ports[n].is_ssl ? "https" : "http";
		const char *host;

		if ((ports[n].protocol & 1) == 1)
		{
			/* IPv4 */
			host = "127.0.0.1";
			printf("Browse files at %s://%s:%i/\n", proto, host, ports[n].port);
			printf("Run example at %s://%s:%i%s\n",
			       proto,
			       host,
			       ports[n].port,
			       MAIN_URI);
		}
	}

	/* Wait until the server should be closed */
	sleep(100);

	mg_stop(ctx);
	printf("Server stopped.\n");
}

int log_message(const struct mg_connection *conn, const char *message)
{
	(void)conn;
	puts(message);
	return 1;
}

void TerminateApp(int status)
{
	pthread_join(uartRxThreadHandler, NULL);
	pthread_join(uartTxThreadHandler, NULL);
	exit(status);
}
