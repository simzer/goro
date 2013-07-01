#BLACK="gnugo --mode gtp"
#WHITE="bin/goro --black gtp --white test"
WHITE="gnugo --mode gtp"
BLACK="bin/goro --black test --white gtp"
TWOGTP="gogui-twogtp -black \"$BLACK\" -white \"$WHITE\""
gogui -verbose -program "$TWOGTP" -computer-both
