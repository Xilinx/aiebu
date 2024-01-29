class Column:
    def __init__(self, col_num, page_num, text, data):
        self.col_num = col_num
        self.data = []

    def insertdata(self, line):
        self.data.append(line)
