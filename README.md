# **Gyr_searchEngine**
## Описание

Поисковый движок **Gyr_searchEngine** -консольное приложение, которое считывает запросы, индексирует документы, загруженные пользователем, сортирует по релевантности и записывает результат в `json` формате.
## Как работает **Gyr_searchEngine**
### Старт    
- при запуске **Gyr_searchEngine** проверит файл конфигурации на валидность и выведет в консоль название движка и его текущую версию.
### Загрузка документов
- программа из файла конфигурации прочитает названия документов, по которым будет осуществляться поиск.
### Запросы
- **Gyr_searchEngine** прочитает запросы из файла `requests.json`.
### Работа
- сначала поисковый движок проиндексирует каждый документ в отдельном потоке и заполнит частотный словарь.Частотный словарь содержит информацию о том сколько раз слово повторяется в каждом документе.
- затем по текущему запросу расчитает относительную релевантность документов и отсортирует их.
- сохранит идентификационный номер документа с его относительной релевантностью в файл `answers.json`.
- выведет содержимое этого файла в консоль.Можно будет увидеть релевантность документов по каждому запросу
- завершит работу.
