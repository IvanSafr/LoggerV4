import os

# --- НАСТРОЙКИ ---

# Имя выходного файла
OUTPUT_FILENAME = 'full_project_context.txt'

# Папки, которые нужно ИГНОРИРОВАТЬ
EXCLUDED_DIRS = {
    'btn',
    'Adafruit_SSD1306',
    'Adafruit_AHTX0',
    'Adafruit_lib',
    'Adafruit_GFX',
    # Стандартные папки, которые обычно не нужны
    '.git',
    '.pio',
    '.vscode',
    '__pycache__',
    'build'
}

# Расширения файлов, которые нужно ВКЛЮЧАТЬ в контекст
# (чтобы случайно не открыть картинки или бинарные файлы как текст)
ALLOWED_EXTENSIONS = {
    '.cpp', '.h', '.c', '.hpp', 
    '.py', '.txt', '.md', '.ini', 
    '.json', '.xml'
}

def generate_context():
    project_root = os.getcwd()
    output_path = os.path.join(project_root, OUTPUT_FILENAME)
    
    print(f"Генерация контекста из: {project_root}")
    print(f"Исключенные папки: {', '.join(EXCLUDED_DIRS)}")
    
    with open(output_path, 'w', encoding='utf-8') as outfile:
        # Добавляем дерево файлов в начало для наглядности
        outfile.write("=== PROJECT STRUCTURE ===\n")
        for root, dirs, files in os.walk(project_root):
            # Фильтрация папок (in-place modification of dirs)
            dirs[:] = [d for d in dirs if d not in EXCLUDED_DIRS]
            
            level = root.replace(project_root, '').count(os.sep)
            indent = ' ' * 4 * (level)
            outfile.write(f"{indent}{os.path.basename(root)}/\n")
            subindent = ' ' * 4 * (level + 1)
            for f in files:
                if is_allowed_file(f):
                    outfile.write(f"{subindent}{f}\n")
        outfile.write("\n" + "="*30 + "\n\n")

        # Чтение содержимого файлов
        for root, dirs, files in os.walk(project_root):
            # Фильтрация папок
            dirs[:] = [d for d in dirs if d not in EXCLUDED_DIRS]
            
            for file in files:
                if not is_allowed_file(file):
                    continue
                
                # Пропускаем сам выходной файл и скрипт генерации
                if file == OUTPUT_FILENAME or file == os.path.basename(__file__):
                    continue

                file_path = os.path.join(root, file)
                rel_path = os.path.relpath(file_path, project_root)
                
                print(f"Обработка: {rel_path}")
                
                outfile.write(f"START OF FILE: {rel_path}\n")
                outfile.write("-" * 20 + "\n")
                
                try:
                    with open(file_path, 'r', encoding='utf-8', errors='ignore') as infile:
                        content = infile.read()
                        outfile.write(content)
                except Exception as e:
                    outfile.write(f"\n[Error reading file: {e}]\n")
                
                outfile.write(f"\n" + "-" * 20 + "\n")
                outfile.write(f"END OF FILE: {rel_path}\n\n")

    print(f"Готово! Контекст сохранен в: {OUTPUT_FILENAME}")

def is_allowed_file(filename):
    return any(filename.endswith(ext) for ext in ALLOWED_EXTENSIONS)

if __name__ == "__main__":
    generate_context()