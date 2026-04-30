# Minishell

## 📌 Overview

Minishell is a custom Unix shell written in C, designed to replicate core Bash behavior.
The project focuses on process management, parsing, and low-level system interaction.

---

## ⚙️ Features

* Execution of commands and binaries
* Pipes (`|`) and redirections (`<`, `>`, `>>`)
* Environment variables (`$PATH`, `$HOME`, etc.)
* Built-in commands (cd, echo, pwd, export, unset, exit)
* Signal handling (Ctrl+C, Ctrl+D, Ctrl+)

---

## 🧠 Key Concepts

* Process creation (`fork`)
* Program execution (`execve`)
* File descriptors and pipes
* Parsing (tokenization and command structure)
* Memory management in C

---

## 🛠 Tech Stack

* C
* Linux (POSIX)
* Makefile

---

## ▶️ Usage

```bash
make
./minishell
```

---

## 💡 Example

```bash
echo hello | grep h > file.txt
```

---

## 📈 What I Learned

* How shells work internally
* Managing multiple processes
* Handling signals and user input
* Writing robust low-level code

---

## 🚀 Future Improvements

* Better parsing (quotes, edge cases)
* History support
* Advanced shell features
