obs <- read.table("pf.data", header = FALSE)
pf <- read.table("pf.out", header = TRUE)
plot(obs[,1], obs[,2])
lines(pf$pos.x, pf$pos.y)
