import serial
import time


def salvar_dados_serial(porta, baudrate, arquivo_saida, duracao):
    """
    Lê dados do monitor serial e salva em um arquivo de texto.


    :param porta: Porta serial do ESP32 (ex: "COM3")
    :param baudrate: Velocidade de comunicação serial (ex: 115200)
    :param arquivo_saida: Nome do arquivo para salvar os dados
    :param duracao: Duração da leitura em segundos
    """
    try:
        # Conecta à porta serial
        ser = serial.Serial(porta, baudrate, timeout=1)
        print(f"Conectado à porta {porta} com baudrate {baudrate}.")


        # Abre o arquivo para escrita
        with open(arquivo_saida, 'w') as arquivo:
            print(f"Salvando dados no arquivo {arquivo_saida}...")
           
            # Marca o início da leitura
            tempo_inicio = time.time()


            while time.time() - tempo_inicio < duracao:
                if ser.in_waiting > 0:
                    # Lê a linha do monitor serial
                    linha = ser.readline().decode('utf-8').strip()
                    print(linha)  # Exibe no console
                    arquivo.write(linha + '\n')  # Salva no arquivo


        print(f"Leitura concluída. Dados salvos em {arquivo_saida}.")
    except serial.SerialException as e:
        print(f"Erro ao acessar a porta serial: {e}")
    except Exception as e:
        print(f"Erro inesperado: {e}")
    finally:
        # Fecha a conexão serial
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("Conexão serial encerrada.")


if __name__ == "__main__":
    # Configurações
    porta_serial = "COM3"       # Substitua pela porta serial correta
    baudrate = 115200           # Velocidade da serial configurada no ESP32
    arquivo_saida = "dados_sensores.txt"  # Nome do arquivo de saída
    duracao_leitura = 48      # Tempo de leitura em segundos (1 minuto + margem)


    salvar_dados_serial(porta_serial, baudrate, arquivo_saida, duracao_leitura)


