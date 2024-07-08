# Word Morph

## Introduction

This project aims to generate "paths" of least cost between words. A path between two words of the same length, given as the start and end points, is a sequence of words of equal length where each word is obtained from its predecessor by replacing one or more characters. All words in the path must belong to a given dictionary.

## Problem Statement

The objective is to find a sequence of words that starts with a given word and ends with another given word. Each intermediate word in the sequence must be obtained by changing one or more letters, as long as the word is on the dictionary. Changing **n** letters has quadratic cost $n^2$.

For example, a path between the words "carro" and "pista" could be:
- carro
- corro
- corto
- porto
- porta
- posta
- pista

This has total cost $6 * 1^2 = 6$. Going from **carro** directly to **pista** would have cost $5^2 = 25$

## Program Description

The program creates linked lists of words of same length based on the given dictionary and then uses Dijkstra's algorithm and an Heap to search for the path with least cost, if it exists.

## Compilation

To compile the program type make on your terminal in the project's file directory.

```sh
make
```

## Program Execution

### Invocation

To execute the program:
```sh
./wordmorph [DICTIONARY] [WORDS]
```
- `wordmorph`: The executable name.
- `DICTIONARY`: A `.dic` file with a list of words, one per line.
- `WORDS`: A `.pal` file containing two words and max n letter changes at a time.

### Output Format

The output is a `.path` file with starting word followed by the cost of the path or **-1** if there is no available path.
Then one word per line until the last word of the path.

Adicional files with problems will be added so you can test it.
