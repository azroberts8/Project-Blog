# Cross Compile Deno to Raspberry Pi
__THIS IS A FAILED PROJECT!__
## Defining the problem
In this project I will be attempting to build [Deno](https://github.com/denoland/deno) from source using an x86-64 Ubuntu machine to run on a 64-bit Raspberry Pi 3 or 4. Previously I have natively compiled Deno on my 8gb Raspberry Pi 4 and am able to execute the binary flawlessly on both the Raspberry pi 3 and 4. __Attempting to compile on Raspberry Pi 3 failed every time due to lack of memory__ however it is able to execute the final binary flawlessly.

## Why am I attempting this?
The Deno runtime is an amazing alternative to any Node.js project and personally I prefer using it to node for its simplicity and lack of bloat in comparison to Node. Deno currently does not officially support ARM platforms so if you wish to run it on the Raspberry Pi you will need to build from source on the Raspberry Pi to install any updates (which are frequent for Deno). This is very intensive on small CPUs such as these and I feel it may have caused hardware damage on my own Raspberry Pi 4 when I performed this given by the crackling noises coming from my CPU as it cooled down after shutting down my device and disconnecting power. For this reason, going forward I would like to use a more powerful device such as my laptop or desktop to compile the updates that I can then run on a low-power Raspberry Pi.

## Steps taken
For this project I will be using Ubuntu 22.04 LTS x86-64 running in a Multipass VM with 4 CPU cores and 8GB memory on my Intel MacBook. At the time of writing this the latest version of Deno is v1.29.2 which __does not__ officially support ARM or the Raspberry Pi.

### The basics - apt update & upgrade
```sh
sudo apt update
sudo apt upgrade
```

### Install ARM64 toolchain
Locate your host system under releases on the [ARM GNU Toolchain Downloads](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) page. We are looking for the AArch64 GNU/Linux target that pertains to our host system. If you are using Ubuntu 64-bit (like myself) we will locate the latest build for x86_64 Linux hosted cross toolcain. I have included with exact download I used in this repository.

Download and unpack the ARM toolchain tarball
```sh
curl <link address> -o aarch64-toolchain.tar.xz
tar xf aarch64-toolchain.tar.xz
```
_If cURL gives a message saying that the file has moved, repeat the command with the address it has provided it has moved to._

Install the extracted binaries by moving them to the /usr directory
```sh
cd <extracted folder name>
sudo cp -r ./bin/* /usr/bin
sudo cp -r ./include/* /usr/include
sudo cp -r ./lib/* /usr/lib
sudo cp -r ./lib64/* /usr/lib64
sudo cp -r ./libexec/* /usr/libexec
sudo cp -r ./share/* /usr/share
```

### Add ARM64 compile target to Rust
If you do not have Rust installed at this point, follow the [Rust installation here](https://www.rust-lang.org/tools/install).

Add the ARM64 Linux target to Rust
```sh
rustup target add aarch64-unknown-linux-gnu
```
I also installed the ARM64 Rust toolchain but I am not sure if it does anything as it is being depreciated
```sh
rustup toolchain install stable-aarch64-unknown-linux-gnu
```

### Clone and compile Deno source code
Clone the latest Deno release from their Github repository
```sh
git clone https://github.com/denoland/deno.git
```
Build the source code for the Raspberry Pi using Cargo
```sh
cd deno
RUST_BACKTRACE=1 V8_FROM_SOURCE=1 cargo build --target=aarch64-unknown-linux-gnu --release
```
_I have included the backtrace flag for more verbose error reports in this step_

__This is where I run into an error__

### The Issue
When the compiler reaches the v8 engine dependency it encounters the following error:
```
error: failed to run custom build command for `v8 v0.60.0`

Caused by:
  process didn't exit successfully: `/home/ubuntu/multipass/deno-arm64/deno/target/release/build/v8-c980af775dddff84/build-script-build` (exit status: 101)

...

  thread 'main' panicked at 'called `Result::unwrap()` on an `Err` value: Os { code: 2, kind: NotFound, message: "No such file or directory" }', /home/ubuntu/.cargo/registry/src/github.com-1ecc6299db9ec823/v8-0.60.0/build.rs:284:8
```
_I have omitted some of the message to spare your reading_

When I attempt to build without the v8 soruce flag I encounter what appears to be the same issue with a later dependency shown below:
```
error: failed to run custom build command for `ring v0.16.20`

Caused by:
  process didn't exit successfully: `/home/ubuntu/multipass/deno-arm64/deno/target/release/build/ring-e5188de20bc2ebfd/build-script-build` (exit status: 101)
```
_Once again omitting much of the message for your reading_

I find it odd that in both cases it state that a file is missing but provides a line reference in the supposed missing files. I checked the provided directories and the files mentioned do indeed exist. Upon opening the files and inspecting the line where the error occurs there does not appear to be references to other files which could be misplaced.

## Summing Up
Considering that Deno easily compiles to the Raspberry Pi natively I am perplexed as to why cross compiling from an x86 machine would not work. If there is any tooling or points that I missed please feel welcome to email me at <andrew@azroberts.com> as I would love to see this project working!