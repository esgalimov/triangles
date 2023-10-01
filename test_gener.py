from random import uniform as rand

with open("test.txt", "w") as f:
    print(100_000, file=f)
    for i in range(1, 100_001):
        print(f"{round(rand(-1000, 1000), 1)} {round(rand(-1000, 1000), 1)} {round(rand(-1000, 1000), 1)}", file=f, end=" ")
        print(f"{round(rand(-1000, 1000), 1)} {round(rand(-1000, 1000), 1)} {round(rand(-1000, 1000), 1)}", file=f, end=" ")
        print(f"{round(rand(-1000, 1000), 1)} {round(rand(-1000, 1000), 1)} {round(rand(-1000, 1000), 1)}", file=f, end=" ")
        if i % 60 == 0:
            print(file=f)
