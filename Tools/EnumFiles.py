import os

def walk(adr):
    mylist=[]
    for root,dirs,files in os.walk(adr):
        for name in files:
            adrlist=os.path.join(root, name)
            mylist.append(adrlist)
    return mylist


def main():
	flist = walk('SceModules')
	for fname in flist:
		print(fname)

main()