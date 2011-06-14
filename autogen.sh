#!/bin/bash

aclocal 
autoconf
automake --add-missing --copy --gnu --include-deps 
