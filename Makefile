
run:
	./main

crun:
	g++ *.cpp -lreadline -o main && ./main

merge:
	python3 merge.py

clean:
	rm -f main