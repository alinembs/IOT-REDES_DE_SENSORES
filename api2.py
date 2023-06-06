from flask_mysqldb import MySQL
from flask import Flask, request, jsonify


app = Flask(__name__)

# Configurações do banco de dados MySQL
app.config['MYSQL_HOST'] = '127.0.0.1'
app.config['MYSQL_USER'] = 'root'
app.config['MYSQL_PASSWORD'] = '12345'
app.config['MYSQL_DB'] = 'esp32'
app.config['MYSQL_CURSORCLASS'] = 'DictCursor'

# Configuração da API Key
API_KEY = 'API_TESTE'


# Inicialização do objeto MySQL
mysql = MySQL(app)

# Rota para autenticação com API Key


@app.route('/autenticar', methods=['POST'])
def autenticar():
    if 'X-API-Key' not in request.headers:
        return jsonify({'error': 'Chave de API ausente.'}), 401

    chave_api = request.headers['X-API-Key']
    if chave_api != API_KEY:
        return jsonify({'error': 'Chave de API inválida.'}), 401

    return jsonify({'message': 'Autenticação bem-sucedida.'}), 200

# Rota para verificar a conexão com o banco de dados


@app.route('/verificar-conexao', methods=['GET'])
def verificar_conexao():
    try:
        cur = mysql.connection.cursor()
        cur.execute('SELECT 1')
        cur.close()
        return jsonify({'message': 'Conexão com o banco de dados estabelecida.'}), 200
    except Exception as e:
        return jsonify({'error': 'Erro ao conectar-se ao banco de dados.', 'details': str(e)}), 500

@app.route('/dados', methods=['POST'])
def receber_dados_sensores():
    # Verifica se a chave de API está presente no cabeçalho da requisição
    if 'X-API-Key' not in request.headers:
        return jsonify({'error': 'Chave de API ausente.'}), 401

    # Verifica se o corpo da requisição contém os campos esperados
    expected_fields = ['nome', 'idade']
    if not request.is_json or not all(field in request.json for field in expected_fields):
        return jsonify({'error': 'Dados inválidos.'}), 400

    nome = request.json['nome']
    idade = request.json['idade']
   
  # Realiza validações adicionais nos campos
    if not nome or not idade:
        return jsonify({'error': 'Dados inválidos.'}), 400

    # Exemplo de impressão dos dados recebidos
    print(f"IDADE: {idade}anos")
    print(f"Nome: {nome}")

    return jsonify({'message': 'Dados recebidos com sucesso.'}), 200
if __name__ == '__main__':
    app.run(host='0.0.0.0')
