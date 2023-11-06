import random

#points in big triangle on x+y+z=0
def big_trian_pnts():
    with open("13.dat", 'w') as test_file:
        print(1000, file=test_file)
        for i in range(3):
            x = random.randint(0, 10_000)
            y = random.randint(0, 10_000)
            z = -x - y

            print(f'{x} {y} {z}', file=test_file)

        for i in range(999):
            x = random.randint(0, 10_000)
            y = random.randint(0, 10_000)
            z = -x - y

            print(f'{x} {y} {z}', file=test_file)
            print(f'{x} {y} {z}', file=test_file)
            print(f'{x} {y} {z}', file=test_file)

#10 x+y+z=a planes and 10_000 triangles on them
def parallel_plenes_trians():
    with open("27.dat", 'w') as test_file:
        print(100_000, file=test_file)
        delta = 0.001

        for i in range(10):
            for j in range(10_000):
                x = random.randint(0, 10_000)
                y = random.randint(0, 10_000)

                for k in range(3):
                    r = random.randint(0, 100)
                    z = delta * i - (x + r) - (y + r)
                    print(f"{round(x + r, 5)} {round(y + r, 5)} {round(z, 5)}", file=test_file)

#line segments on plane x+y+z=0
def segs_on_plane():
    with open("15.dat", 'w') as test_file:
        print(10_000, file=test_file)

        for i in range(10_000):
            x = random.randint(0, 10_000)
            y = random.randint(0, 10_000)


            r  = random.randint(0, 1000)
            z = -(x + r) - (y + r)
            print(f'{x + r} {y + r} {z}', file=test_file)
            print(f'{x + r} {y + r} {z}', file=test_file)

            r  = random.randint(0, 1000)
            z = -(x + r) - (y + r)
            print(f'{x + r} {y + r} {z}', file=test_file)

#segs on line x=y=z
def segs_on_line():
    with open("16.dat", 'w') as test_file:
        print(10_000, file=test_file)
        for i in range(10_000):
            x = random.randint(0, 100_000)
            r = 2

            print(f'{x + r} {x + r} {x + r}', file=test_file)
            print(f'{x + r} {x + r} {x + r}', file=test_file)
            print(f'{x - r} {x - r} {x - r}', file=test_file)


#triangles; one of segs is on same line
def triags_on_line():
    with open("17.dat", 'w') as test_file:
        print(10_000, file=test_file)
        for i in range(10_000):
            x = random.randint(0, 100_000)
            r = 2

            print(f'{x + r} {x + r} {x + r}', file=test_file)
            print(f'{random.randint(0, 100_000)} {random.randint(0, 100_000)} {random.randint(0, 100_000)}', file=test_file)
            print(f'{x - r} {x - r} {x - r}', file=test_file)

#big_trian_pnts()
#parallel_plenes_trians()
#segs_on_plane()
#segs_on_line()
#triags_on_line()
