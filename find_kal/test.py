from random import uniform as rand

# for i in range(40):
#     with open("t" + str(i) + ".txt", "w") as f:
#         print(10_000, file=f)
#         for i in range(1, 10_001):
#             place = [round(rand(-10000, 10000), 1), round(rand(-10000, 10000), 1), round(rand(-10000, 10000), 1)]
#
#             print(f"{round(rand(-100, 100) + place[0], 1)} {round(rand(-100, 100) + place[1], 1)} {round(rand(-100, 100) + place[2], 1)}", file=f, end=" ")
#             print(f"{round(rand(-100, 100) + place[0], 1)} {round(rand(-100, 100) + place[1], 1)} {round(rand(-100, 100) + place[2], 1)}", file=f, end=" ")
#             print(f"{round(rand(-100, 100) + place[0], 1)} {round(rand(-100, 100) + place[1], 1)} {round(rand(-100, 100) + place[2], 1)}", file=f, end=" ")
#             if i % 60 == 0:
#                 print(file=f)

for i in range(40):
    print(f'"t{i}.txt"', end=' ')
