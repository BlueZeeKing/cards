# How to install ncurses

## 1: Build ncurses

Clone the ncurses source code with

```bash
git clone https://github.com/mirror/ncurses
```

then, in the `ncurses` repository, configure the build process to install in the correct place and with the correct settings

```bash
./configure --enable-widec --prefix=$HOME/.local
```

then, run make to complete the build.

```bash
make -j8
```

## 2. Install ncurses

In the `ncurses` repository, install into the `~/.local` directory by running

```bash
make install
```

then, tell `g++` where we installed the `ncurses` by adding the following to `~/.zshrc`

```bash
export CPATH="$HOME/.local/include:$CPATH"
export LIBRARY_PATH="$HOME/.local/lib:$LIBRARY_PATH"
```

then, update any running shells with

```bash
source ~/.zshrc
```

All new shells will have the new settings already applied
