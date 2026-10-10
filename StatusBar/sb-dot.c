#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// Function to parse XML and extract the first float
float parseXml(const char *xml) {
    const char *start = strstr(xml, "<s t=");
    if (start == NULL) {
        fprintf(stderr, "Error: Unable to find '<s t=' in XML.\n");
        exit(EXIT_FAILURE);
    }

    start = strchr(start, '>') + 1;
    const char *end = strchr(start, '<');
    if (end == NULL) {
        fprintf(stderr, "Error: Unable to find closing '<' in XML.\n");
        exit(EXIT_FAILURE);
    }

    char *floatStr = strndup(start, end - start);
    if (floatStr == NULL) {
        fprintf(stderr, "Error: Memory allocation failure.\n");
        exit(EXIT_FAILURE);
    }

    float result = strtof(floatStr, NULL);
    free(floatStr);
    return result;
}

// Function to map a float to RGB color components
void mapToColor(float variance, unsigned char *red, unsigned char *green, unsigned char *blue) {
    if (variance < 50) {
        *red = (unsigned char)(255 * (100 - variance) / 100);
        *green = 255;
        *blue = 0;
    } else {
        *red = 0;
        *green = (unsigned char)(255 * (variance - 50) / 50);
        *blue = (unsigned char)(255 * (variance - 50) / 50);
    }
}

int main() {
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Error: Unable to initialize CURL.\n");
        return EXIT_FAILURE;
    }

    curl_easy_setopt(curl, CURLOPT_URL, "https://gcpdot.com/gcpindex.php");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite);

    FILE *responseFile = tmpfile();
    if (!responseFile) {
        fprintf(stderr, "Error: Unable to create temporary file.\n");
        return EXIT_FAILURE;
    }

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, responseFile);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "Error: %s\n", curl_easy_strerror(res));
        return EXIT_FAILURE;
    }

    fseek(responseFile, 0, SEEK_SET);
    char buffer[1024];
    size_t bytesRead = fread(buffer, 1, sizeof(buffer), responseFile);
    buffer[bytesRead] = '\0';

    float variance = parseXml(buffer);

    unsigned char red, green, blue;
    mapToColor(variance, &red, &green, &blue);

    printf("^c#%02X%02X%02X^", red, green, blue);

    fclose(responseFile);
    curl_easy_cleanup(curl);

    return 0;
}
