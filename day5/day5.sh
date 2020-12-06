#!/bin/bash

tr BFRL 1010 < input.txt |sort|tee >(echo Part one: $((2#`tail -n1`))>&2) | echo Part two: $((1+2#`uniq -d -s9`))
