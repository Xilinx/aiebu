class Operation:
    def __init__(self, name, args):
        self.name = name.lower()
        self.args = [x.strip() for x in args.strip().split(',')] if args is not None else []

    def __str__(self):
        return f"{self.name} {self.args}"

