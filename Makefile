# rotina de tarefas do Makefile
all: task_all
task_all: create_executable generate_tag_cloud_html generate_tag_cloud_html_color generate_tag_cloud_number version help
	echo "Tarefas executadas com sucesso"

# rotina de criação do executável
create_executable:
	@gcc main.c -o tag_cloud_generator
	echo "Criando executavel"

# rotina de geração da tag cloud html
generate_tag_cloud_html:
	-./tmp @./tag_cloud_generator -h Teste.txt
	echo "Gerando tag cloud html"

#rotina de geração de tag cloud com cor
generate_tag_cloud_html_color:
	-./tmp @./tag_cloud_generator -h -color Teste.txt
	echo "Gerando tag cloud html com cor" 

#rotina de geração de tag cloud com numero
generate_tag_cloud_number:
	-./tmp @./tag_cloud_generator -c -r 5 Teste.txt
	echo "Gerando tag cloud com numero"

#exebindo versão do tag cloud
version:
	-./tmp @./tag_cloud_generator --v
	echo "Exibindo versão do tag cloud"

#exibindo ajuda do tag cloud
help:
	-./tmp @./tag_cloud_generator -help
	echo "Exibindo ajuda do tag cloud"