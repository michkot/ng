# make all methods virtual:
```
^(\s+)([^/\s]+.+);(.*)
$1virtual $2 = 0;$3
```
# make all virtual methods virtual override :
```
^(\s+virtual )([^/\s]+.+) = 0(;.*)
$1$2 override$3
```
