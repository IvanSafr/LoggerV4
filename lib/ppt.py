import os

# --- НАСТРОЙКИ ---
# Папки, которые нужно пропустить
EXCLUDE_DIRS = {'.pio', '.vscode', '.git', '__pycache__', 'test', "Adafruit_AHTX0", "Adafruit_GFX", "Adafruit_lib", "Adafruit_SSD1306", "btn"}
# Расширения файлов, которые нужно включить
INCLUDE_EXTENSIONS = {'.h', '.cpp', '.c', '.ini', '.txt'}
# Имя выходного файла
OUTPUT_FILE = 'project_context.txt'

def generate_context(root_dir):
    with open(OUTPUT_FILE, 'w', encoding='utf-8') as out_f:
        out_f.write(f"PROJECT STRUCTURE AND SOURCE CODE\n")
        out_f.write(f"Generated from: {os.path.abspath(root_dir)}\n")
        out_f.write("="*50 + "\n\n")

        # 1. Сначала записываем дерево файлов (для понимания структуры)
        out_f.write("FILE TREE:\n")
        for root, dirs, files in os.walk(root_dir):
            dirs[:] = [d for d in dirs if d not in EXCLUDE_DIRS]
            level = root.replace(root_dir, '').count(os.sep)
            indent = ' ' * 4 * level
            out_f.write(f"{indent}{os.path.basename(root)}/\n")
            sub_indent = ' ' * 4 * (level + 1)
            for f in files:
                if any(f.endswith(ext) for ext in INCLUDE_EXTENSIONS):
                    out_f.write(f"{sub_indent}{f}\n")
        
        out_f.write("\n" + "="*50 + "\n\n")

        # 2. Затем записываем содержимое каждого файла
        for root, dirs, files in os.walk(root_dir):
            dirs[:] = [d for d in dirs if d not in EXCLUDE_DIRS]
            
            for file in files:
                if any(file.endswith(ext) for ext in INCLUDE_EXTENSIONS):
                    file_path = os.path.join(root, file)
                    relative_path = os.path.relpath(file_path, root_dir)
                    
                    out_f.write(f"--- FILE: {relative_path} ---\n")
                    try:
                        with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                            out_f.write(f.read())
                    except Exception as e:
                        out_f.write(f"[Error reading file: {e}]")
                    
                    out_f.write("\n\n")

    print(f"Готово! Контекст сохранен в файл: {OUTPUT_FILE}")

if __name__ == "__main__":
    # Запускаем в текущей директории
    generate_context('.')