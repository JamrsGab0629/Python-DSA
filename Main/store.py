class Store:
    def __init__(self):
        self._storage = {}

    def create(self, name, price):
        if name in self._storage:
            raise ValueError(f"{name} already exists in the storage")
        self._storage[name] = price

    def update(self, name, price):
        if name not in self._storage:
            raise KeyError(f"{name} doesn't exist in the storage")
        self._storage[name] = price

    def read(self, name):
        return self._storage.get(name)

    def delete(self, name):
        if name not in self._storage:
            raise KeyError(f"{name} doesn't exist yet, try adding it")
        del self._storage[name]

    def list_all(self):
        return dict(self._storage)


s = Store()
s.create("egg", 10)
s.create("bread", 20)
s.create("coke", 15)
print(s.list_all())    