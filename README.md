# LOTRcppgame

<p>O mundo está em guerra. De um lado, o exército vermelho, comandado pelo terrível Sauron, um ser de poder incomensurável que deseja dominar todas as terras. Ao seu lado, lutam orcs ferozes e sanguinários, um cavaleiro das trevas chamado Darknight, que possui uma lâmina capaz de matar com um só golpe, e uma árvore gigante chamada Arvrok, que espalha o terror e a confusão entre os inimigos com seus galhos e raízes. Do outro lado, o exército branco, liderado por um sábio e poderoso mago, que protege seus aliados com um escudo mágico impenetrável. Junto com ele, combatem bravos anões e humanos, armados com machados e espadas, um elfo curandeiro que tem o dom da benção universal, capaz de curar e fortalecer seus companheiros, e um guerreiro místico da água, que usa sua espada para contra-atacar os golpes dos adversários. A batalha final está prestes a começar. Qual lado você vai escolher?<p>
  
<h1>Manual de execução:</h1>

<ul>
  <li>Os soldados devem ser inseridos no arquivo "soldier_....txt" no seu respectivo exército;</li>
  <li>A primeira linha do arquivo deve conter o atributo "União" do exército, que deve ser apenas um número entre 0 e 10;</li>
  <li>Os soldados devem ser inseridos após o atributo união, separados por linhas: Da seguinte maneira: <br> <b>Classe nome vida(float) poder(float)</b>, o espaço é obrigatório entre cada elemento;</li>
  <li>O nome do soldado não deve conter espaços;</li>
  <li>As classes podem ser: <br> Exército do bem: <b>Mago, Humano, Guerreiro, Elfo, Anao</b> <br> Exército do mal: <b>Sauron, Orc, Darknight, Arvrok</b><br>Elas devem ser escritas exatamente como acima;</li>
  <li>Você pode modificar a variável definida DELAY se quiser acompanhar a batalha em tempo real no terminal, o tempo deve estar em milisegundos;</li>
  <li>Todo o decorrer da batalha ficará salvo no arquivo "battle_log.txt";</li>
</ul>
