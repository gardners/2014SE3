cat */*.txt | sort | awk '{ if ($1 == last) { n=n+1; } else { print n,last; n=1; last=$1; } } END { print n,last; }' | sort -n
