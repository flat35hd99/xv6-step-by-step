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

## マルチコアでHello, World!

`run.sh`と`start.c`のCPUSの宣言を`1`から`2`へ変更して実行すると以下のように`hello, world`が連続して表示されます。

```
$ ./run.sh 
hello, world
hello, world
```

問題なさそうです。ここから、cpu数に応じて、表示される`hello, world`の数が比例して増加すると予想できます。CPUS=3のときを試すと

```
$ ./run.sh 
hhello, world
ello, world
hello, world
```

残念ながら崩れてしまいました。これはきっと、複数のコアが同時に一つのUARTのバッファかレジスタ、またはその両方へ書き込みを行っているために生じる現象だと推察されます。いわゆる、"race condition"です。