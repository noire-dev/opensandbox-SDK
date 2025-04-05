#!/bin/bash
grep -Ilr . ../code/ | xargs dos2unix
