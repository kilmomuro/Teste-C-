using SupermercadoApp.Services;

var service = new SupermercadoService();

while (true)
{
    Console.Clear();
    Console.WriteLine("=== Sistema de Supermercado ===");
    Console.WriteLine("1 - Cadastrar produto");
    Console.WriteLine("2 - Listar produtos");
    Console.WriteLine("3 - Repor estoque");
    Console.WriteLine("4 - Registrar venda");
    Console.WriteLine("5 - Relatório de vendas");
    Console.WriteLine("0 - Sair");
    Console.Write("Escolha uma opção: ");

    var opcao = Console.ReadLine();

    try
    {
        switch (opcao)
        {
            case "1":
                CadastrarProduto(service);
                break;
            case "2":
                ListarProdutos(service);
                break;
            case "3":
                ReporEstoque(service);
                break;
            case "4":
                RegistrarVenda(service);
                break;
            case "5":
                MostrarRelatorio(service);
                break;
            case "0":
                return;
            default:
                Console.WriteLine("Opção inválida.");
                Pausar();
                break;
        }
    }
    catch (Exception ex)
    {
        Console.WriteLine($"Erro: {ex.Message}");
        Pausar();
    }
}

static void CadastrarProduto(SupermercadoService service)
{
    Console.Write("Código: ");
    var codigo = Console.ReadLine() ?? string.Empty;

    Console.Write("Nome: ");
    var nome = Console.ReadLine() ?? string.Empty;

    Console.Write("Preço: ");
    var preco = decimal.Parse(Console.ReadLine() ?? "0");

    Console.Write("Estoque inicial: ");
    var estoque = int.Parse(Console.ReadLine() ?? "0");

    service.CadastrarProduto(codigo, nome, preco, estoque);

    Console.WriteLine("Produto cadastrado com sucesso.");
    Pausar();
}

static void ListarProdutos(SupermercadoService service)
{
    var produtos = service.ListarProdutos();

    if (produtos.Count == 0)
    {
        Console.WriteLine("Nenhum produto cadastrado.");
        Pausar();
        return;
    }

    Console.WriteLine("\nProdutos cadastrados:");
    foreach (var p in produtos)
    {
        Console.WriteLine($"- [{p.Codigo}] {p.Nome} | Preço: {p.Preco:C} | Estoque: {p.QuantidadeEstoque}");
    }

    Pausar();
}

static void ReporEstoque(SupermercadoService service)
{
    Console.Write("Código do produto: ");
    var codigo = Console.ReadLine() ?? string.Empty;

    Console.Write("Quantidade para reposição: ");
    var quantidade = int.Parse(Console.ReadLine() ?? "0");

    service.ReporEstoque(codigo, quantidade);

    Console.WriteLine("Estoque atualizado com sucesso.");
    Pausar();
}

static void RegistrarVenda(SupermercadoService service)
{
    var itens = new Dictionary<string, int>(StringComparer.OrdinalIgnoreCase);

    while (true)
    {
        Console.Write("Código do produto (vazio para finalizar): ");
        var codigo = Console.ReadLine() ?? string.Empty;

        if (string.IsNullOrWhiteSpace(codigo))
        {
            break;
        }

        Console.Write("Quantidade: ");
        var quantidade = int.Parse(Console.ReadLine() ?? "0");

        if (itens.ContainsKey(codigo))
        {
            itens[codigo] += quantidade;
        }
        else
        {
            itens[codigo] = quantidade;
        }
    }

    var venda = service.RegistrarVenda(itens);

    Console.WriteLine($"Venda registrada! ID: {venda.Id}");
    Console.WriteLine($"Total: {venda.Total:C}");
    Pausar();
}

static void MostrarRelatorio(SupermercadoService service)
{
    var vendas = service.ListarVendas();

    if (vendas.Count == 0)
    {
        Console.WriteLine("Nenhuma venda registrada.");
        Pausar();
        return;
    }

    Console.WriteLine("\nÚltimas vendas:");
    foreach (var venda in vendas)
    {
        Console.WriteLine($"Venda {venda.Id} - {venda.DataHora:g} - Total: {venda.Total:C}");
        foreach (var item in venda.Itens)
        {
            Console.WriteLine($"   * {item.NomeProduto} ({item.CodigoProduto}) x{item.Quantidade} = {item.Subtotal:C}");
        }
    }

    Console.WriteLine($"\nFaturamento total: {service.ObterFaturamentoTotal():C}");
    Pausar();
}

static void Pausar()
{
    Console.WriteLine("\nPressione ENTER para continuar...");
    Console.ReadLine();
}
