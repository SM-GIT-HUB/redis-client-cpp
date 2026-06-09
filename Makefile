
com:
	g++ *.cpp -lreadline -lssl -lcrypto -o main

run:
	./main

crun:
	g++ *.cpp -lreadline -lssl -lcrypto -o main && ./main

merge:
	python3 merge.py

clean:
	rm -f main