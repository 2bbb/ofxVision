#!/bin/bash

cd $(dirname $0)
pwd
xattr -cr *.app
