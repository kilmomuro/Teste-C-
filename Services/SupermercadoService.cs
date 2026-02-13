using SupermercadoApp.Models;

namespace SupermercadoApp.Services;

public class SupermercadoService
{
    private readonly Dictionary<string, Produto> _produtos = new(StringComparer.OrdinalIgnoreCase);
    private readonly List<Venda> _vendas = new();

    public IReadOnlyCollection<Produto> ListarProdutos() => _produtos.Values.OrderBy(p => p.Nome).ToList();

    public Produto? BuscarProduto(string codigo)
    {
        _produtos.TryGetValue(codigo, out var produto);
        return produto;
    }

    public void CadastrarProduto(string codigo, string nome, decimal preco, int quantidadeEstoque)
    {
        if (string.IsNullOrWhiteSpace(codigo) || string.IsNullOrWhiteSpace(nome))
        {
            throw new ArgumentException("Código e nome do produto são obrigatórios.");
        }

        if (preco <= 0)
        {
            throw new ArgumentException("O preço deve ser maior que zero.");
        }

        if (quantidadeEstoque < 0)
        {
            throw new ArgumentException("O estoque inicial não pode ser negativo.");
        }

        if (_produtos.ContainsKey(codigo))
        {
            throw new InvalidOperationException("Já existe um produto com esse código.");
        }

        var produto = new Produto(codigo, nome, preco, quantidadeEstoque);
        _produtos.Add(codigo, produto);
    }

    public void ReporEstoque(string codigo, int quantidade)
    {
        var produto = BuscarProduto(codigo) ?? throw new KeyNotFoundException("Produto não encontrado.");
        produto.ReporEstoque(quantidade);
    }

    public Venda RegistrarVenda(Dictionary<string, int> itens)
    {
        if (itens.Count == 0)
        {
            throw new ArgumentException("A venda precisa de pelo menos um item.");
        }

        foreach (var (codigo, quantidade) in itens)
        {
            var produto = BuscarProduto(codigo) ?? throw new KeyNotFoundException($"Produto com código {codigo} não encontrado.");
            if (quantidade <= 0)
            {
                throw new ArgumentException("A quantidade do item deve ser positiva.");
            }

            if (produto.QuantidadeEstoque < quantidade)
            {
                throw new InvalidOperationException($"Estoque insuficiente para {produto.Nome}.");
            }
        }

        var venda = new Venda();

        foreach (var (codigo, quantidade) in itens)
        {
            var produto = BuscarProduto(codigo)!;
            produto.RetirarEstoque(quantidade);

            venda.AdicionarItem(new ItemVenda
            {
                CodigoProduto = produto.Codigo,
                NomeProduto = produto.Nome,
                Quantidade = quantidade,
                PrecoUnitario = produto.Preco
            });
        }

        _vendas.Add(venda);
        return venda;
    }

    public decimal ObterFaturamentoTotal() => _vendas.Sum(v => v.Total);

    public IReadOnlyCollection<Venda> ListarVendas() => _vendas.OrderByDescending(v => v.DataHora).ToList();
}
