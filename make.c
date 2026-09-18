#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

#define eprintf(...) fprintf(stderr, __VA_ARGS__)

#define AOCLIBS_ABORT(msg, ...)                                    \
    (fprintf(stderr, "%s: %s:%u: ", __func__, __FILE__, __LINE__), \
     fprintf(stderr, msg " " __VA_ARGS__),                         \
     fputc('\n', stderr),                                          \
     abort())

#ifdef NDEBUG
#define ASSERT(...)
#else
#define ASSERT(expr, ...) \
    ((expr) ? (void)0 : AOCLIBS_ABORT("Assertion failed: " #expr, __VA_ARGS__))
#endif

#define ASSERT_NONNULL(exp) ASSERT((exp), "passing NULL pointer to Nonnull parameter")

// ===

#define MAX_FLAGS 64

char GLOBAL_BUFFER[256] = { 0 };

char *args[256];
size_t args_count = 0;

enum {
    NONE = 0,
    BINARY = 1,
    OBJECT = 2,
};

bool needs_build(const char *source_file, const char *object) {
    struct stat sf = { 0 };
    struct stat o = { 0 };

    if (stat(source_file, &sf) == -1) {
        if (errno == ENOENT) return true;

        eprintf("error: failed to stat %s (%s)\n", source_file, strerror(errno));
        return true;
    }

    if (stat(object, &o) == -1) {
        if (errno == ENOENT) return true;

        eprintf("error: failed to stat %s (%s)\n", object, strerror(errno));
        return true;
    }

    return sf.st_mtime > o.st_mtime;
}

static int run(char **argv) {
    ASSERT_NONNULL(argv);
    pid_t pid = fork();
    if (pid == -1) {
        eprintf("error: failed to run %s\n", argv[0]);
        exit(1);
    }
    if (pid == 0) {
        execvp(argv[0], argv);
        exit(127);
    }

    args_count = 0;

    int status;
    if (waitpid(pid, &status, 0) == -1) return 1;
    if (WIFEXITED(status)) return WEXITSTATUS(status);
    return 1;
}

void push(char *arg) {
    args[args_count++] = arg;
}

int compile_file(char *file, char **flags, int type) {
    size_t file_len = strlen(file);

    ASSERT(file[file_len - 1] == 'c', "not a C source file");

    memcpy(GLOBAL_BUFFER, file, file_len);
    GLOBAL_BUFFER[file_len - 2] = '\0';
    char *output_file = GLOBAL_BUFFER;

    bool needs_compilation = false;

    if (type == NONE) return 0;

    needs_compilation = needs_build(file, output_file);
    if (!needs_compilation) return 0;

    push("gcc");

    if (flags) {
        for (; *flags; flags++) {
            push(*flags);
        }
    }

    push(file);

    if (type == BINARY) {
        push("-o");
        push(output_file);
    } else if (type == OBJECT) {
        push("-c");
    }

    push(NULL);

#if 0
	char **args2 = args;
	for (; *args2; args2++) {
		eprintf("arg: %s\n", *args2);
	}
#endif

    eprintf("Compiling %s...\n", file);
    return run(args);
}

char **get_flags(char *file, int *type) {
    static char line[1024];
    static const char *flags[MAX_FLAGS];

    FILE *fp = fopen(file, "r");
    if (!fp) return NULL;

    if (!fgets(line, sizeof(line), fp)) {
        fclose(fp);
        return NULL;
    }

    fclose(fp);

    size_t header_len = 7;

    if (strncmp(line, "// obj:", header_len) == 0) {
        *type = OBJECT;
    } else if (strncmp(line, "// bin:", header_len) == 0) {
        *type = BINARY;
    } else {
        *type = NONE;
        return NULL;
    }

    size_t argc = 0;

    char *tok = strtok(line + header_len, " \t\r\n");
    while (tok && argc + 1 < MAX_FLAGS) {
        flags[argc++] = tok;
        tok = strtok(NULL, " \t\r\n");
    }

    flags[argc] = NULL;

    return (char **)flags;
}

int compile(char *file) {
    int type = NONE;
    char **flags = NULL;
    flags = get_flags(file, &type);
    return compile_file(file, flags, type);
}

void compile_all(void) {
    DIR *dir = opendir(".");
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *ent;

    while ((ent = readdir(dir))) {
        char *name = ent->d_name;
        size_t len = strlen(name);

        if (len < 3) continue;

        if (strcmp(name + len - 2, ".c") != 0) continue;

        int err = compile(name);

        if (err != 0) eprintf("%s failed (%d)\n", name, err);
    }
    closedir(dir);
}

int main(void) {
    compile("flag_generator.c");

    char *flag_generator[] = { "./flag_generator", NULL };
    if (run(flag_generator) != 0) return 1;

    compile("dpacker.c");

    return 0;
}
