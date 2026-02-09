# Pipex

A small UNIX-style pipeline reimplementation (42 project).
It reproduces the behavior of:

```bash
< infile cmd1 | cmd2 > outfile
```

Using low-level syscalls: `pipe()`, `fork()`, `dup2()`, and `execve()`.

---

## Project Structure

- `pipex.c` — process creation + piping logic (the core)
- `utils.c` — command parsing, `$PATH` lookup, `execve()` launching
- `Includes/pipex.h` — headers + prototypes
- `Libft/` — custom libft dependency (built automatically by the Makefile)

---

## How to Compile

From inside the project folder:

```bash
make
```

This builds:
- `Libft/libft.a`
- `pipex` executable

Clean targets:

```bash
make clean   # removes .o
make fclean  # removes .o and pipex
make re      # full rebuild
```

---

## How to Run

### Usage

```bash
./pipex <infile> "<cmd1>" "<cmd2>" <outfile>
```

### Example

```bash
./pipex infile "cat" "wc -l" outfile
```

Equivalent shell command:

```bash
< infile cat | wc -l > outfile
```

---

## Most Important Parts of the Code

### 1) The pipeline + forks (`pipex.c`)

The heart of the project is `pipex()`:

- Creates a pipe: `pipe(fd)`
- Forks **two children**
- **Child 1** reads from `infile` and writes into the pipe
- **Child 2** reads from the pipe and writes into `outfile`
- Parent closes pipe fds and `waitpid()`s for both children

Key idea: `dup2()` rewires standard input/output so commands behave like in a real shell pipeline.

**Child 1**
- Opens `infile`
- Redirects stdin → infile
- Redirects stdout → pipe write end
- Executes `cmd1`

**Child 2**
- Opens `outfile`
- Redirects stdin → pipe read end
- Redirects stdout → outfile
- Executes `cmd2`

---

### 2) Command execution + PATH resolution (`utils.c`)

The other critical piece is `execute()`:

1. Splits the command string by spaces
2. If command starts with `/`, executes directly
3. Otherwise searches `$PATH`
4. Runs `execve()`

If anything fails, it prints an error and exits.

---

## Notes / Limitations

- Commands are split only by spaces; shell quoting is not fully supported
- Requires a valid environment (`envp`) for `$PATH` resolution

---

## Quick Test

```bash
echo -e "hello\nworld\n42" > infile
./pipex infile "cat" "wc -l" outfile
cat outfile
```

Expected output:

```text
3
```
