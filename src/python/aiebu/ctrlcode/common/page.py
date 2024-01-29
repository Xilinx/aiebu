class Page:
    def __init__(self, col_num, page_num, text, data, islastpage):
        self.col_num = col_num
        self.page_num = page_num
        self.text = text
        self.data = data
        self.islastpage = islastpage

    def __str__(self):
        return f"Col_num:{self.col_num}\tpage_num:{self.page_num}\ttext:{self.text}\t\
                data:{self.data}\tislastpage:{self.islastpage}"

