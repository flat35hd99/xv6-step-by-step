package xv6stepbystep_test

import (
	"context"
	"io"
	"os/exec"
	"strings"
	"testing"
	"time"
)

func TestPrintHellowWorld(t *testing.T) {
	expected := "Hello, World!"

	ctx, cancel := context.WithTimeout(context.Background(), 3*time.Second)
	defer cancel()

	cmd := exec.CommandContext(ctx, "qemu-system-riscv64", "-machine", "virt", "-bios", "none", "-kernel", "kernel/kernel", "-m", "128M", "-smp", "4", "-nographic", "-global", "virtio-mmio.force-legacy=false")

	stdout, err := cmd.StdoutPipe()
	if err != nil {
		t.Fatal(err)
	}
	defer stdout.Close()

	if err := cmd.Start(); err != nil {
		t.Fatal(err)
	}
	defer cmd.Wait()

	resultBufProcessing := []byte{}
	bufSize := 4096
	buf := make([]byte, bufSize)
	isSucessed := false
	for {
		n, err := stdout.Read(buf)

		resultBufProcessing = append(resultBufProcessing, buf[:n]...)

		// Write test here
		result := string(resultBufProcessing)
		if strings.Contains(result, expected) {
			isSucessed = true
			cancel()
			break
		}

		if err == io.EOF {
			break
		}
	}

	if !isSucessed {
		actual := string(resultBufProcessing)
		if !strings.Contains(actual, expected) {
			t.Fatalf("%s expected to be haven but %s does not have.", expected, actual)
		}
	}
}

func TestJustPrintHellowWorld(t *testing.T) {
	expected := "Hello, World!\r\n"

	ctx, cancel := context.WithTimeout(context.Background(), time.Second)
	defer cancel()

	cmd := exec.CommandContext(ctx, "qemu-system-riscv64", "-machine", "virt", "-bios", "none", "-kernel", "kernel/kernel", "-m", "128M", "-smp", "4", "-nographic", "-global", "virtio-mmio.force-legacy=false")

	stdout, err := cmd.StdoutPipe()
	if err != nil {
		t.Fatal(err)
	}
	defer stdout.Close()

	if err := cmd.Start(); err != nil {
		t.Fatal(err)
	}
	defer cmd.Wait()

	resultBufProcessing := []byte{}
	bufSize := 4096
	buf := make([]byte, bufSize)
	isSucessed := false
	for {
		n, err := stdout.Read(buf)

		resultBufProcessing = append(resultBufProcessing, buf[:n]...)

		if err == io.EOF {
			break
		}
	}

	println(string(resultBufProcessing))
	if !isSucessed {
		actual := string(resultBufProcessing)
		if actual != expected {
			t.Fatalf("%s expected but %s got", expected, actual)
		}
	}
}
