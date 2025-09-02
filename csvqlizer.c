#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int csv_row_bytes(char* csv_buffer);

int main(int argc, char* argv[])
{
    // Check if only one file argument was provided
    if (argc != 2)
    {
        printf("Usage: ./csvparser FILENAME\n");
        return 1;
    }
    FILE* csv = fopen(argv[1], "r");
    // Check the fopen succeeded
    if (csv == NULL)
    {
        printf("fopen failed\n");
        return 2;
    }
    // Determine how many bytes are in the first line of the CSV
    int bytes = 0;
    char character;
    while ((character = fgetc(csv)) != EOF)
    {
        // Add a byte each time that fgetc returns a non-EOF character
        bytes++;
    }
    // Bring us back to the start of the file
    rewind(csv);
    // Allocate a buffer for the csv to be read into
    char* csv_buffer = malloc(bytes);
    // Check that csv_buffer returned a value
    if (csv_buffer == NULL)
    {
        printf("malloc for the csv_buffer failed\n");
        return 3;
    }
    // Read the entire file into the buffer 
    fread(csv_buffer, 1,  bytes, csv);
    // Determine how many bytes are in the first line of the CSV
    int first_row_bytes = csv_row_bytes(csv_buffer);
    // Create a new string for just the first CSV row
    char* first_row = malloc(first_row_bytes);
    if (first_row == NULL)
    {
        printf("malloc for the first_row failed\n");
        return 3;
    }
    // Copy the first_row from the csv_buffer
    for (int i = 0; i < first_row_bytes; i++)
    {
        first_row[i] = csv_buffer[i];
    }
    // Create a buffer for the fixed_first_row
    char* fixed_first_row = malloc(first_row_bytes);
    if (fixed_first_row == NULL)
    {
        printf("malloc for the fixed_first_row failed\n");
        return 3;
    }
    // Fix the first row, skipping quotations and fixing disallowed characters
    // Names in SQL must begin with a letter (a-z) or underscore (_). Subsequent characters in a name can be letters, digits (0-9), or underscores
    // Initialize the bytes of the first row
    int fixed_first_row_bytes = 0;
    for (int i = 0; i < first_row_bytes; i++)
    {
        // If the character is a comma, copy it over
        if (first_row[i] == ',')
        {
            fixed_first_row[fixed_first_row_bytes] = first_row[i];
            fixed_first_row_bytes++;
        }
        // If the character is allowed in a table_name, add it to the fixed first_row 
        else if ((first_row[i] == 95) || ((first_row[i] <= 90) && (first_row[i] >= 65)) || ((first_row[i] <= 122) && (first_row[i] >= 97)) || ((first_row[i] <= 57) && (first_row[i] >= 48)))
        {
            // Set that character to be the next in the fixed_first_row
            fixed_first_row[fixed_first_row_bytes] = first_row[i];
            fixed_first_row_bytes++;
        }
        // If the character is a quotes, skip it, we don't need it
        else if ((first_row[i] == '"') || (first_row[i] == '\n'))
        {
           // Do nothing
        }
        // If the character is a disallowed character
        else
        { 
            fixed_first_row[fixed_first_row_bytes] = '_';
            fixed_first_row_bytes++;
        }
    }
    // Print out the new first row to replace in the csv
    for (int i = 0; i < fixed_first_row_bytes; i++)
    {
        printf("%c", fixed_first_row[i]);
    }
    // Print a newline to separate column list and varchar columns
    printf("\n");
    // Print out the new deployment table varchar columns
    for (int i = 0; i < fixed_first_row_bytes; i++)
    {
        if (fixed_first_row[i] == ',')
        {
            printf(" varchar,\n");
        }
        else
        {
            printf("%c", fixed_first_row[i]);
        }
    }
    printf(" varchar\n");
    // Create a new file with the new first CSV row
    // Print the SQL deployment table with every column as a varchar
    // Free everything
    free(csv_buffer);
    free(first_row);
    free(fixed_first_row);
    // Close the the file when you are done
    fclose(csv);
}

int csv_row_bytes(char* csv_buffer)
{
    int csv_row_bytes = 0;
    // Detect if we are using windows or linux line endings
    bool is_linux = true;
    // Loop through csv_buffer until we find the end of a CSV_row
    char current = 'a';
    bool quote = false;
    if (is_linux)
    {
        while (current != '\n')
        {
            // Set the first byte the CSV equal to current
            current = csv_buffer[csv_row_bytes];
            // If we encouter a quotation mark, set the quote flag equal to true
            if (current == '"')
            {
                // This will just flicker it to be the opposite of what it was
                quote = !quote;
            }
            // if the current flag is a new line but we are within quotes, ensure current doesn't break the while loop
            if ((current == '\n') && (quote))
            {
                current = 'a';
            }
            // Increment the amount of bytes in the row of the csv
            csv_row_bytes++;
        }
    }
    else if (!is_linux)
    {
        printf("Windows support not developed yet.\n");
        return 4;
    }
    return csv_row_bytes;
}

