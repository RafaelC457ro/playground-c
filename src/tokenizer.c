#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum command_type
{
    GET,
    SET,
    DELETE,
    INVALID
};

typedef struct command
{
    enum command_type type;
    char *key;
    char *value;
} Command;

static void s_to_lower_case(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = tolower(str[i]);
    }
}

int parseCommand(const char *input, Command *cmd)
{
    int max_len = 3;
    char *args[3];
    char *token;
    char *input_copy = strdup(input);

    int i = 0;
    while ((token = strsep(&input_copy, " ")) != NULL && i < 3)
    {
        args[i] = strdup(token);
        i++;
    }

    if (i == 1)
    {
        s_to_lower_case(args[0]);

        cmd->type = INVALID;
        cmd->key = NULL;
        cmd->value = NULL;
        return -1;
    }

    if (i == 2)
    {
        s_to_lower_case(args[0]);
        if (strcmp(args[0], "get") == 0)
        {
            cmd->type = GET;
            cmd->key = strdup(args[1]);
            cmd->value = NULL;
            return 0;
        }
        else if (strcmp(args[0], "delete") == 0)
        {
            cmd->type = DELETE;
            cmd->key = strdup(args[1]);
            cmd->value = NULL;
            return 0;
        }
    }
    else if (i == 3)
    {
        s_to_lower_case(args[0]);
        if (strcmp(args[0], "set") == 0)
        {
            cmd->type = SET;
            cmd->key = strdup(args[1]);
            cmd->value = strdup(args[2]);
            return 0;
        }
    }

    if (input_copy != NULL)
        free(input_copy);

    return -1;
}

void freeCommand(Command *cmd)
{
    free(cmd->key);
    free(cmd->value);
}

int main()
{
    char input[100];
    Command cmd;
    int s;

    printf("Enter command: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    s = parseCommand(input, &cmd);

    if (s == -1)
    {
        printf("Invalid command.\n");
        return 0;
    }

    if (cmd.type == GET)
    {
        printf("Command Type: GET\n");
        printf("Key: %s\n", cmd.key);
    }
    else if (cmd.type == SET)
    {
        printf("Command Type: SET\n");
        printf("Key: %s\n", cmd.key);
        printf("Value: %s\n", cmd.value);
    }
    else if (cmd.type == DELETE)
    {
        printf("Command Type: DELETE\n");
        printf("Key: %s\n", cmd.key);
    }
    else
    {
        printf("Invalid command.\n");
    }

    freeCommand(&cmd);

    return 0;
}