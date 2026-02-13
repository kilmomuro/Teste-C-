namespace SupermercadoApp.Models;

public class ItemVenda
{
    public required string CodigoProduto { get; init; }
    public required string NomeProduto { get; init; }
    public int Quantidade { get; init; }
    public decimal PrecoUnitario { get; init; }
    public decimal Subtotal => Quantidade * PrecoUnitario;
}
