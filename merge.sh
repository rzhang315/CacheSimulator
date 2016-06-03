BLOCK_SIZES="32 64 128 256 512 1024 2048"
CACHE_SIZES="131072"
ASSOCIATIVITIES="4"
TRACES=`ls trace.merge`

for b in $BLOCK_SIZES; do
  for s in $CACHE_SIZES; do
    for a in $ASSOCIATIVITIES; do
      for t in $TRACES; do
        echo -n "$t, $b, $s, $a, \n"
        ./cachesim $t $b $s $a
      done
    done  
  done
done
