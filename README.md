# boxer_orangePi
Projekt centrali sterownika uprawy roślin oparty o platformę Orange Pi Zero

- serwer HTTP, obsługa zapytań POST/GET
- obsługa bazy danych sqlite3, zapis parametrów uprawy
- transmisja RS485 (UART), odbiór
- użycie biblioteki pthread.h, wielowątkowość
- logowanie określane definami
