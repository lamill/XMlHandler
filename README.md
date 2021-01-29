## Тестовое задание для собеседования

### ТЗ [ссылка](https://drive.google.com/file/d/1PvqF65HAO7I2cpEPafXltq5zRbuC1BUT/view?usp=sharing)

### особенности реализации
* размеры элементов окон не были настроены, поскольку не было необходимости
* проверка xml файлов реализована посредством xsd
* xsd файл прилагается, перед началом работы необходимо импортировать посредством пользовательского интерфейса
* поскольку не было уточнений по поводу формата полей расширений и кодировок  в xml файлах - они были унифицированы, регулярка находится в xsd файле
* данные о форматах и кодировке хрянятся в БД в виде строк только для упрощения задачи, поскольку не было оговорено подробностей. 
* во многих функциях не осуществлялись проверки на корректность данных, поскольку в них не было необходимости в рамках тестового задания . Были реализованы минимальные проверки для корректной работы приложения.
* работа с БД вшита в код модели, реализован минимальный функционал, необходимый для ТЗ
* хотел проверить через valgrind, но он начал ругаться на qt и его утечки, даже при пустом проекте, поэтому пропустил этот момент
* делал doxygen документацию 
