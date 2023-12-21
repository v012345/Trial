from PIL import Image

def concatenate_images(image_paths, output_path):
    # 打开所有的WebP格式图片
    images = [Image.open(image_path).convert('RGBA') for image_path in image_paths]

    # 获取每张图片的大小
    widths, heights = zip(*(i.size for i in images))

    # 计算总宽度和高度
    total_width = sum(widths)
    max_height = max(heights)

    # 创建一张新的图像，大小为总宽度和最大高度
    result_image = Image.new('RGBA', (total_width, max_height), (255, 255, 255, 0))

    # 在新图像上粘贴每张图片
    current_width = 0
    for image in images:
        result_image.paste(image, (current_width, 0), image)
        current_width += image.width

    # 保存拼接后的图片
    result_image.save(output_path)

if __name__ == "__main__":
    for i in range(247,248):
        # print(i)
        # 替换为你的WebP格式图片路径列表
        image_paths = [ str(i)+"_1.webp",str(i)+ "_2.webp",str(i)+ "_3.webp",str(i)+"_4.webp",str(i)+"_5.webp",str(i)+"_6.webp",]
    
        # # 替换为输出拼接后图片的路径
        output_path ="book/"+ str(i) + ".png"

        # 执行拼接操作
        concatenate_images(image_paths, output_path)
   