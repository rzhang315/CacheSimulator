BLOCK_SIZES="32 64 128 256 512 1024 2048"
CACHE_SIZES="131072"
ASSOCIATIVITIES="1 2 4 8 16 32 64"
TRACES=`ls trace.bubble trace.merge`

for b in $BLOCK_SIZES; do
  for s in $CACHE_SIZES; do
    for a in $ASSOCIATIVITIES; do
      for t in $TRACES; do
        echo -n "$t, $s, $a, $b, "
        ./cachesim $t $b $s $a
      done
    done  
  done
done
