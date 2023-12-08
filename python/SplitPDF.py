import PyPDF2


def split_pdf(input_path, output_path, start_page, end_page):
    with open(input_path, "rb") as file:
        # 创建PDF阅读器对象
        pdf_reader = PyPDF2.PdfReader(file)

        # 创建PDF写入器对象
        pdf_writer = PyPDF2.PdfWriter()
        # 循环遍历指定页面范围，将页面添加到写入器对象
        for page_num in range(start_page - 1, min(end_page, len(pdf_reader.pages))):
            page = pdf_reader.pages[page_num]
            pdf_writer.add_page(page)

        # 以写入二进制模式打开输出文件，并将写入器对象中的页面写入
        with open(output_path, "wb") as output_file:
            pdf_writer.write(output_file)


# 示例用法
input_file_path = "../log/游戏引擎架构（第2版） (Jason Gregory) (Z-Library).pdf"
output_file_path = "2.pdf"
start_page_number = 1
end_page_number = 3

split_pdf(input_file_path, "1.pdf", 1, 260)
# split_pdf(input_file_path, "2.pdf", 296, 548)
# split_pdf(input_file_path, "3.pdf", 549, 999)
