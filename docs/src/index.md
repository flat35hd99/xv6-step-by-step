---
title: xv6をゼロから作る
---

[xv6のRISC-V版](mit-pdos/xv6-riscv)をインクリメンタルに作成するにはどうやればいいのか調べます。少しずつ作成するにあたり、常になにかしらのOSが動き、かつそれが正常であることを検証できる自動テストをメンテしながら開発する方法を模索します。

## Hello, world

QEMU上で動く、"Hello, world!" OSを作成します。

QEMU上で動かすのはブートローダを自分で書いたり、仕様を調べたりしなくてよいため、比較的簡単です。`-kernel`オプションで渡したelfファイルをメモリ上に展開してくれるため、適切なメモリ配置になるようにすればよいです。そのため、メモリ配置を指示するためのリンクファイルと、プログラムファイル本体を用意することになります。

本家では、それぞれ`kernel.ld`と`entry.S`, `main.c`が対応しています。このプロジェクトでも同じファイル名で作成します。

初期化は以下の流れで行われています。

- `main.c/main()`
  - `console.c/consoleinit()`
    - `console.c/uartinit()`
  - `printf.c/printfinit()`

`printf()`の中身から、`uart.c`のいろいろが使われてるのがわかるので、それらを呼び出せるようにします。

コンパイルと実行は、それぞれ `compile.sh`と`run.sh`を実行することで行えます。どちらも本家のMakefileから持ってきています。

```console
$ cd /path/to/xv6-step-by-step
$ ./compile.sh
$ ./run.sh
hello, world
```

実行すると、"hello, world"の文字列が表示されました🎉🎉🎉

### おさらい

- QEMUでHello, Worldに成功した。
- とりあえず動かすため、細かいお話、たとえばUARTの仕様やロック周りについては触れなかった。
- CPU数は1つで実行した。
