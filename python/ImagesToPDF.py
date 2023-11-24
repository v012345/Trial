from reportlab.pdfgen import canvas
from reportlab.lib.pagesizes import letter
from reportlab.lib.units import inch
from PIL import Image


def images_to_pdf(image_paths, output_pdf_path):
    # 创建一个PDF文件
    pdf_canvas = canvas.Canvas(output_pdf_path, pagesize=letter)

    # 设置PDF页面尺寸
    pdf_width, pdf_height = 1212, 1495
    pdf_canvas.setPageSize((pdf_width, pdf_height))

    # 遍历图像路径并将每个图像添加到PDF中
    for image_path in image_paths:
        # 将图像添加到PDF
        pdf_canvas.drawImage(image_path, 0, 0, width=pdf_width, height=pdf_height)

        # 添加新的页面
        pdf_canvas.showPage()

    # 保存PDF文件
    pdf_canvas.save()


# 图像文件路径列表
image_paths = []
for i in range(1, 462):
    image_paths.append("D:/Trial/job/" + str(i) + ".png")

# 输出PDF文件路径
output_pdf_path = "output_images.pdf"

# 调用函数
images_to_pdf(image_paths, output_pdf_path)
