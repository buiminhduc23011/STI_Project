import os
from PIL import Image, ImageDraw

def create_progressbar_image(progress, width=700, height=30, bar_color="#007BFF", bg_color="lightgrey"):
    """
    Creates a progress bar image.
    
    Args:
    - progress (float): Progress percentage (0 to 100).
    - width (int): Total width of the image.
    - height (int): Total height of the image.
    - bar_color (str): Color of the progress bar (hex color code).
    - bg_color (str): Background color.
    
    Returns:
    - PIL Image: Image of the progress bar.
    """
    # Create a blank image with background color
    image = Image.new("RGB", (width, height), color=bg_color)
    draw = ImageDraw.Draw(image)
    
    # Calculate bar width
    bar_width = int((progress / 100) * width)
    
    # Draw the progress bar
    draw.rectangle([0, 0, bar_width, height], fill=bar_color)
    
    return image

# Create directory 'Processbar' if it doesn't exist
output_folder = "Processbar"
os.makedirs(output_folder, exist_ok=True)

# Create and save images for progress from 0% to 100%
for progress in range(0, 101, 1):
    img = create_progressbar_image(progress, width=700, height=30)  # bar_color default is #007BFF
    img.save(os.path.join(output_folder, f"{progress}.png"))  # Save images in 'Processbar' folder

print(f"All images have been saved in the '{output_folder}' folder!")
