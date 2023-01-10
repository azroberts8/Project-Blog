# Cross Compile Deno to Raspberry Pi
__THIS IS A FAILED PROJECT!__
## Defining the problem
In this project I will be attempting to build [Deno](https://github.com/denoland/deno) from source using an x86-64 Ubuntu machine to run on a 64-bit Raspberry Pi 3 or 4. Previously, I have natively compiled Deno on my 8gb Raspberry Pi 4 and am able to execute the binary flawlessly on both the Raspberry pi 3 and 4. __Attempting to compile on Raspberry Pi 3 failed every time due to lack of available memory__ however the Raspberry Pi 3 is able to execute the Deno binary flawlessly.

## Why am I attempting this?
The Deno runtime is an amazing alternative to Node.js for web application projects and personally I prefer using it for its simplicity and reduced project size in comparison to Node. At the time of writing this Deno does not officially support ARM platforms so if you wish to run it on the Raspberry Pi you will need to build Deno from source on the Raspberry Pi. Additionally, to install any updates (which are frequent for Deno) you will need to build the entire application from source again. This is a very intensive process on small devices like the Raspberry Pi and I am afraid I may have caused hardware damage on my own device from doing so. During the later stage of compiling, the CPU of my Rapsberry Pi 4 began to make a faint crackling noise that resembled a bowl of Rice Krispies and proceeded to make this noise for a few minutes after shutting the pi down and disconnecting it from power. Granted, I did not have a heatsink installed on my pi but regardless, going forward I would like to use a more capable device such as my laptop or desktop to compile the Deno updates that I can then run on a low-power Raspberry Pi. Seems simple enough.

## Steps taken
For this project I will be using Ubuntu 22.04 LTS x86-64 running in a Multipass VM with 4 CPU cores and 12GB memory on my Intel MacBook. At the time of writing this the latest version of Deno is v1.29.2 which __does not__ officially support ARM or the Raspberry Pi.

### The basics - apt update & upgrade
```sh
sudo apt update
sudo apt upgrade
```

### Install ARM64 toolchain
Locate your host system under the releases section on the [ARM GNU Toolchain Downloads](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) page. We are looking for the AArch64 GNU/Linux target specific to our host system. In my case I am using Ubuntu 64-bit as my host operating system so I will install the ```arm-gnu-toolchain-12.2.rel1-x86_64-aarch64-none-linux-gnu.tar.xz``` package. _Your version and system may differ._ Copy the respective link address and paste it into the cURL command below.

Download and unpack the ARM toolchain tarball
```sh
curl <link address> -o aarch64-toolchain.tar.xz
tar xf aarch64-toolchain.tar.xz
```
_If cURL gives a message stating that the file has moved, try the command again but instead the address it has moved to in your URL._

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
_For our "installation" I am just moving the binaries to their respective locations in our host root directory_

### Add ARM64 compile target to Rust
If you do not have Rust installed at this point, follow the [Rust installation here](https://www.rust-lang.org/tools/install) to install all the tools you need to build Rust applications.

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
_I have omitted some of the stdout and stderr to spare your reading_

When I attempt to build without the v8 soruce flag I encounter what appears to be the same issue with a later dependency shown below:
```
error: failed to run custom build command for `ring v0.16.20`

Caused by:
  process didn't exit successfully: `/home/ubuntu/multipass/deno-arm64/deno/target/release/build/ring-e5188de20bc2ebfd/build-script-build` (exit status: 101)
```
_Once again omitting some of the message for your reading_

I find it odd that in both cases it state that a file is missing but provides a line reference in the supposed missing files. I checked the provided directories and the files mentioned do indeed exist. Upon opening the files and inspecting the line where the error occurs there does not appear to be references to additional files which could have been misplaced.

## Summing Up & Potential Future Solution
Considering that Deno easily compiles to the Raspberry Pi natively I am perplexed as to why cross compiling from an x86 machine does not work. If there is any tooling or steps that I am missing please feel welcome to email me at <andrew@azroberts.com>. I would love to see this project working!

Another potential solution that I have yet to try and seems much less elegant and appealing could involve emulating a Raspberry Pi environment on your x86 machine using a tool such as QEMU and natively compiling Deno in that emulated environment. I have not used QEMU before to know whether something like this could work however, at a high level this could theoretically work with a minor performance hit due to emulation.