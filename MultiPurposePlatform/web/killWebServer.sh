#!/bin/bash
portFind="$1"
echo "Finding process on port: $portFind"
pid=$(netstat -tulnp | grep :"$1" | awk '{print $7}' | cut -f1 -d"/")
echo "Process found: $pid"
kill -9 $pid
echo "Process $pid killed"