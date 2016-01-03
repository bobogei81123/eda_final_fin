
with open('qq') as f:
    it = iter(f)

    while True:
        x = next(it)
        if x.startswith('UNDETECTED'):
            break

    s = set()
    while True:
        x = next(it).strip()
        if x.startswith('Running'):
            break

        s.add(x)

ans = set()
with open('circuits/s35932_r1') as f:
    next(f)
    next(f)
    next(f)

    for l in f:
        ans.add(l.strip())

print(ans - s)
print(s - ans)
        
