#!/bin/bash

printf "[01.01.2025 21:12:45] DEBUG: logger = 0x5f2ce73a3b\n"
printf "[01.01.2025 21:12:46]  INFO: Running server at http://127.0.0.1:8080\n"
printf "[01.01.2025 21:12:47]  WARN: This feature is deprecated.\n"
printf "[01.01.2025 21:12:48] ERROR: Failed to open file \"index.html\": Permission denied\n"
printf "[01.01.2025 21:12:49] FATAL: Failed to open socket: Address already in use\n\n"

printf "\033[90m[01.01.2025 21:12:45]\033[0m \033[1;97m\033[102mDEBUG:\033[0m logger = 0x5f2ce73a3b \n"
printf "\033[90m[01.01.2025 21:12:46]\033[0m \033[1;97m\033[104m INFO:\033[0m Running server at http://127.0.0.1:8080 \n"
printf "\033[90m[01.01.2025 21:12:47]\033[0m \033[1;97m\033[43m WARN:\033[0m \033[33mThis feature is deprecated.\033[0m \n"
printf "\033[90m[01.01.2025 21:12:48]\033[0m \033[1;97m\033[101mERROR:\033[0m \033[91mFailed to open file \"index.html\": Permission denied\033[0m \n"
printf "\033[90m[01.01.2025 21:12:49]\033[0m \033[1;97m\033[41mFATAL:\033[0m \033[1;31mFailed to open socket: Address already in use\033[0m \n\n"

printf "{\"time\": \"2025-01-01T21:12:45+03:00\", \"log_level\": \"DEBUG\", \"text\": \"logger = 0x5f2ce73a3b\"}\n"
printf "{\"time\": \"2025-01-01T21:12:46+03:00\", \"log_level\": \"INFO\", \"text\": \"Running server at http://127.0.0.1:8080\"}\n"
printf "{\"time\": \"2025-01-01T21:12:47+03:00\", \"log_level\": \"WARN\", \"text\": \"This feature is deprecated.\"}\n"
printf "{\"time\": \"2025-01-01T21:12:48+03:00\", \"log_level\": \"ERROR\", \"text\": \"Failed to open file \"index.html\": Permission denied\"}\n"
printf "{\"time\": \"2025-01-01T21:12:49+03:00\", \"log_level\": \"FATAL\", \"text\": \"Failed to open socket: Address already in use\"}\n\n"
