#!/bin/bash
chopem.py libtasn1-git --new-revision=libtasn1_2_11 --old-revision=libtasn1_2_11~20 --log=warning | tee run-example-chopits.sh
sh run-example-chopits.sh
