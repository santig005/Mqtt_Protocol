#include <arpa/inet.h>
#include <stdio.h>
#include <time.h>

void write_log_to_client(char *path, struct sockaddr_in dir_ip, char *request,
                         char *response) {
  // File pointer
  FILE *logFile;

  // Open the log file in append mode
  logFile = fopen(path, "a");

  // Check if the file opened successfully
  if (logFile == NULL) {
    printf("Error opening file!\n");
    return;
  }

  // Get current time
  time_t rawtime;
  struct tm *timeinfo;
  char buffer[80];

  time(&rawtime);
  rawtime -= 5 * 3600;
  timeinfo = gmtime(&rawtime);

  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

  // Write message with timestamp to the log file
  fprintf(logFile, "[%s] %s\n", buffer, inet_ntoa(dir_ip.sin_addr));
  fprintf(logFile, "%s\n", request);
  fprintf(logFile, "%s\n", response);
  fprintf(logFile, "\n");
  // Close the file
  fclose(logFile);
}

void write_log_to_broker(char *path, struct sockaddr_in *dir_ip_client, char *request,char *response) {
  // File pointer
  FILE *logFile;

  // Open the log file in append mode
  logFile = fopen(path, "a");

  // Check if the file opened successfully
  if (logFile == NULL) {
    printf("Error opening file!\n");
    return;
  }

  // Get current time
  time_t rawtime;
  struct tm *timeinfo;
  char buffer[80];

  time(&rawtime);
  rawtime -= 5 * 3600;
  timeinfo = gmtime(&rawtime);

  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

  // Write message with timestamp to the log file
  fprintf(logFile, "[%s] %s\n", buffer, inet_ntoa(dir_ip_client->sin_addr));
  fprintf(logFile, "%s\n", request);
  fprintf(logFile, "%s\n", response);
  fprintf(logFile, "\n");
  // Close the file
  fclose(logFile);
}
