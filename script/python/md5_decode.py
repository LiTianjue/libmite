

import hashlib
m = hashlib.md5()
data = raw_input("input:")
m.update(data)
psw = m.hexdigest()

print psw
