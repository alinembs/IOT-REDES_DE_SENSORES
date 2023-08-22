from flask import Flask, request, jsonify
from flask_mysqldb import MySQL
from datetime import datetime

app = Flask(__name__)

# Configurações do banco de dados MySQL
app.config['MYSQL_HOST'] = '127.0.0.1'
app.config['MYSQL_USER'] = 'root'
app.config['MYSQL_PASSWORD'] = '12345'
app.config['MYSQL_DB'] = 'tcc1'
app.config['MYSQL_CURSORCLASS'] = 'DictCursor'

# Configuração da API Key
API_KEY = 'API_TESTE'

# Inicialização do objeto MySQL
mysql = MySQL(app)

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


# Rota para autenticação com API Key
@app.route('/autenticar', methods=['POST'])
def autenticar():
    if 'X-API-Key' not in request.headers:
        return jsonify({'error': 'Chave de API ausente.'}), 401

    chave_api = request.headers['X-API-Key']
    if chave_api != API_KEY:
        return jsonify({'error': 'Chave de API inválida.'}), 401

    return jsonify({'message': 'Autenticação bem-sucedida.'}), 200

# Rota para adicionar dados ao banco de dados


@app.route('/adicionar-dados', methods=['POST'])
def adicionar_dados():
    if 'X-API-Key' not in request.headers:
        return jsonify({'error': 'Chave de API ausente.'}), 401

    chave_api = request.headers['X-API-Key']
    if chave_api != API_KEY:
        return jsonify({'error': 'Chave de API inválida.'}), 401

    if not request.is_json:
        return jsonify({'error': 'Dados inválidos.'}), 400

    temperatura_c = request.json.get('temperatura_c')
    temperatura_f = request.json.get('temperatura_f')
    umidade = request.json.get('umidade')
    data_hora_str = request.json.get('data_hora')
    data_hora = datetime.strptime(data_hora_str,'%Y-%m-%d %H:%M:%S')
    # Formatando a data/hora para o formato do MySQL
    data_hora_mysql = data_hora.strftime('%Y-%m-%d %H:%M:%S')
    cur = mysql.connection.cursor()
    cur.execute("INSERT INTO Sensores(Data,TemperaturaC,TemperaturaF,Umidade) VALUES (%s, %s, %s, %s)",
                (data_hora_mysql, temperatura_c, temperatura_f, umidade))
    mysql.connection.commit()
    cur.close()

    return jsonify({'message': 'Dados adicionados com sucesso.'}), 200


if __name__ == '__main__':
    app.run(host='0.0.0.0')
