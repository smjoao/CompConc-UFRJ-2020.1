out_file = open("out.txt", "r")
line = out_file.readline();
num = int(line)
while line:
    nextnum = int(line)
    if num > nextnum:
        print("Erro: Número fora de ordem!")
        break
    num = nextnum
    line = out_file.readline()
if not line: print("Corretude validada!")
out_file.close()