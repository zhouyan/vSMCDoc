obs <- read.table("pf.data", header = FALSE)
pf <- read.table("pf.out", header = TRUE)

pdf("pf.pdf")
plot(obs[,1], obs[,2], xlab = "X", ylab = "Y")
lines(pf$pos.0, pf$pos.1)
dev.off()

sink("pf.rout")
print(pf[1:5,])
sink()
