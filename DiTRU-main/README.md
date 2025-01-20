# DiTRU: A Resurrection of NTRU over Dihedral Group
This repository contains the scripts accompanying the article

## Provide reference implementation in C for DiTRU and revised parameter sets of NTRU that achieve the same level of security according to the same evaluation criteria. According to NIST, the provided parameter sets targets the security levels 128-bit, 192-bit, and 256-bit.

* The code is written based on NTRU submission to the first and third round of NIST with the required modifications for DiTRU.

# Requirements

* C compilier like GCC


# Description of files
Short description of the content:
* folder `DiTRU` contains three sub-folders: `ditru2048541`, `ditru4096797`, and `ditru40961039` provides the implementation for DiTRU parameter sets that target the security levels 1, 3, and 5, respectively.

* folder `NTRU` contains three sub-folders:
`ntru2048587`, `ntru2048863`, and `ntru40961109` provide the revised NTRU parameter sets that achieve the same levels of security according to the same evaluation criteria followed for DiTRU.


# How to use

Change the directory to the directory of one of the parameter sets like `ditru2048541`.

Run `make,` and then an executable file will be generated.

Run the executable file by typing in the command line `./main` or `taskset 0x1 ./main` to run the program on a specific core.

The program will be executed, and two files will be generated. 

1. `request file` contains: 
    - `count`: a counter for the generated example.
    - `seed`: a random seet to generate the corresponding key later.
    - `original message`: a message to be encrypted according to the CCA encryption scheme.

2. `response file` for the corresponding request file generates:
    - `sk`: a secret key.
    - `pk`: a public key.
    - `ct`: the ciphertext corresponding to encrypting the message in the request file.
    - `decrypted message`: decrypt the message that is supposed to be equal to the original message.

Finally, the average CPU cycles to generate the keypair, encrypt, and decrypt the message are calculated after generating all the examples.
