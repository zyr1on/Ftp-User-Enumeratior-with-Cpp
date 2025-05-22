# FTP Username Enumeration

Multi-threaded vsFTPD user enumerator written in C++ 17. It checks valid usernames on an FTP server by analyzing FTP response codes.

---
## How to Compile

Make sure you have a C++ compiler installed (e.g., `g++`) and `make` utility.

Run the following command in the terminal to compile the program:
```bash
$ make
```
This will compile the source files and generate the executable named `ftp_user_enum`.

## Usage

```bash
$ ./ftpBrute --ip <target_ip> --file <users_file> [--port <port_number>] [--threads <num_threads>] [--exitWhenFound]
```
## Options

- `--ip <target_ip>`  
  Target FTP server IP address. (**Required**)

- `--file <users_file>`  
  Path to the username list file. (**Required**)

- `--port <port_number>`  
  FTP server port to connect to. Defaults to `21` if not specified.

- `--threads <num_threads>`  
  Number of concurrent threads to use. Defaults to `4`.

- `--exitWhenFound`  
  If specified, the program exits immediately after finding the first valid username.


## Example

```bash
$ ./ftpBrute --ip 10.10.10.10 --file users.txt --threads 4
```
This command runs the FTP brute force tool targeting the FTP server at `10.10.10.10`. It uses the usernames listed in `users.txt`, connects on the default FTP port `21`, and utilizes 4 concurrent threads to speed up the enumeration process. The program will continue testing all usernames without stopping after finding a valid one.

```bash
$ ./ftpBrute --ip 10.10.10.10 --file users.txt --port 21 --threads 4 --exitWhenFound
```
This command enumerates usernames on the FTP server at `10.10.10.10` using 4 threads, reading from users.txt, connecting on port 21, and stops as soon as the first valid user is found.

