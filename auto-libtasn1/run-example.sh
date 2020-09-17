#!/bin/bash
chopem.py libtasn1-git --new-revision=libtasn1_2_11 --old-revision=libtasn1_2_11~20 --log=warning > run-example-chopit.sh
sh run-example-chopit.sh
